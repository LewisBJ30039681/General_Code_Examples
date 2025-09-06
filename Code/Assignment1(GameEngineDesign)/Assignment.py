import random
import math
import pygame
import PyParticles

pygame.init()

clock = pygame.time.Clock()

#screen size
(width, height) = (1000, 1000)
screen = pygame.display.set_mode((width, height))

pygame.display.set_caption('Assignment - Block Boi Denfender')

#colours for ease of use
blue = (0, 0, 255)
yellow = (255, 211, 67)
red = (255, 0, 0)

#player variables
player_size = 40
player_speed = 8

#bullet variables
bullet_width = 20
bullet_height = 20
bullet_speed = 10

#sound variables
bullet_sound = pygame.mixer.Sound("..\Assignment1\Sounds\laser_gun.wav")
bullet_sound.set_volume(0.2)
destroyed_sound = pygame.mixer.Sound("..\Assignment1\Sounds\pop.wav")
destroyed_sound.set_volume(0.2)
player_hit_sound = pygame.mixer.Sound("..\Assignment1\Sounds\player_hit.wav")
player_hit_sound.set_volume(0.2)
game_over_sound = pygame.mixer.Sound("..\Assignment1\Sounds\game_over.wav")
game_over_sound.set_volume(0.2)
level_complete_sound = pygame.mixer.Sound("..\Assignment1\Sounds\yippee.wav")
level_complete_sound.set_volume(0.2)

#game variables
lives = 3
score = 0
level = 1
font = pygame.font.Font(None, 36)

class Player:
    def __init__(self, x, y):
        #player hitbox
        self.rect = pygame.Rect(x, y, player_size, player_size)
        self.bullets = []

    def move(self, keys):
        #player movement
        if keys[pygame.K_LEFT] or keys[pygame.K_a]:
            self.rect.x -= player_speed
        if keys[pygame.K_RIGHT] or keys[pygame.K_d]:
            self.rect.x += player_speed

        #keep player within screen bounds
        self.rect.x = max(0, min(width - player_size, self.rect.x))

    def shoot(self):
        #spawn bullet at players position
        bullet_x = self.rect.x + player_size // 2 - bullet_width // 2
        bullet_y = self.rect.y
        self.bullets.append(Bullet(bullet_x, bullet_y))
        #bullet sound play
        bullet_sound.play()

    def draw(self, screen):
        #draw player
        pygame.draw.rect(screen, blue, self.rect)
        for bullet in self.bullets:
            bullet.draw(screen)

class Bullet:
    def __init__(self, x, y):
        #bullet hitbox
        self.rect = pygame.Rect(x, y, bullet_width, bullet_height)

    def move(self):
        #bullet movement
        self.rect.y -= bullet_speed

    def draw(self, screen):
        #draw bullet
        pygame.draw.rect(screen, yellow, self.rect)

#creating player
player = Player(width // 2, height - player_size - 10)

#creating particle system taking from PyParticles lib
universe = PyParticles.Environment((width, height))
universe.colour = (255, 255, 255)
universe.addFunctions(['move', 'bounce', 'collide', 'accelerate', 'combine'])
universe.acceleration = (math.pi, 0.25)

#number of particles at start of game
num_particles = 5

#spawning particles at random from a range
for _ in range(num_particles):
    particle_type = random.choice(["big", "medium", "small"])

    if particle_type == "big":
        size = random.randint(30, 40)
        mass = random.randint(500, 1500)
        speed = random.uniform(0.5, 2)

    elif particle_type == "medium":
        size = random.randint(20, 30)
        mass = random.randint(300, 800)
        speed = random.uniform(2, 4)

    else:
        size = random.randint(10, 20)
        mass = random.randint(100, 500)
        speed = random.uniform(3, 5)
    
    universe.addParticles(
        x = random.randint(50, width - 50),
        y = random.randint(50, height // 2),
        size = size,
        mass = mass,
        speed = speed,
        angle = random.uniform(0, 2 * 3.1415),
        colour = red
    )

#menu function
def show_menu():
    menu_running = True

    while menu_running:
        screen.fill((0, 0, 0))  #sets background to black

        #render menu text
        title_text = font.render("BLOCK BOI DEFENDER", True, (255, 255, 255))
        start_text = font.render("Press ENTER to Start", True, (255, 255, 255))
        quit_text = font.render("Press ESC to Quit", True, (255, 255, 255))

        #UI text to screen
        screen.blit(title_text, (width // 2 - 150, height // 2 - 100))
        screen.blit(start_text, (width // 2 - 140, height // 2))
        screen.blit(quit_text, (width // 2 - 120, height // 2 + 50))

        pygame.display.flip()

        #event handling for menu
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()
            if event.type == pygame.KEYDOWN:
                #start game
                if event.key == pygame.K_RETURN:
                    menu_running = False
                #quit game
                if event.key == pygame.K_ESCAPE:
                    pygame.quit()
                    exit()

#menu function being called
show_menu()

#game loop variables
running = True
waiting = False

while running:
    #clear the screen
    screen.fill(universe.colour)
    keys = pygame.key.get_pressed()

    #check for player input
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
            player.shoot()

    #check for bullet collisions with particles
    for bullet in player.bullets[:]:
        for particle in universe.particles[:]:
            #create hitbox for particle collision detection
            particle_rect = pygame.Rect(particle.x - particle.size, particle.y - particle.size, particle.size * 2, particle.size * 2)

            if bullet.rect.colliderect(particle_rect):
                print("Particle hit!")
                
                #destroyed sound play
                destroyed_sound.play()

                #split large particles into two medium particles
                if particle.size >= 30:
                    print("Big particle split!")

                    offset_x = 40
                    offset_y = 40
                    
                    universe.addParticles(
                        x = particle.x + offset_x,
                        y = particle.y + offset_y,
                        size = random.randint(20, 30),
                        mass = random.randint(300, 800),
                        speed = random.uniform(2, 4),
                        angle = random.uniform(0, 2 * 3.1415),
                        colour = red
                    )

                    universe.addParticles(
                        x = particle.x - offset_x,
                        y = particle.y - offset_y,
                        size = random.randint(20, 30),
                        mass = random.randint(300, 800),
                        speed = random.uniform(2, 4),
                        angle = random.uniform(0, 2 * 3.1415),
                        colour = red
                    )
                
                #split medium particles into two small ones
                elif 20 <= particle.size < 30:
                    print("Medium particle split into 2 small ones!")

                    offset_x = 40
                    offset_y = 40

                    universe.addParticles(
                        x = particle.x + offset_x,
                        y = particle.y + offset_y,
                        size = random.randint(10, 20),
                        mass = random.randint(100, 500),
                        speed = random.uniform(3, 6),
                        angle = random.uniform(0, 2 * 3.1415),
                        colour = red
                    )

                    universe.addParticles(
                        x = particle.x - offset_x,
                        y = particle.y - offset_y,
                        size = random.randint(10, 20),
                        mass = random.randint(100, 500),
                        speed = random.uniform(3, 6),
                        angle = random.uniform(0, 2 * 3.1415),
                        colour = red
                    )

                #increase score when a particle is destroyed
                score += 1
                print(f"Score: {score}")

                #remove bullet and destroyed particle
                player.bullets.remove(bullet)  
                universe.particles.remove(particle)  
                break

    #update bullet positions
    player.bullets = [bullet for bullet in player.bullets if bullet.rect.y > 0]
    for bullet in player.bullets:
        bullet.move()

    #remove particles that have merged
    universe.particles = [p for p in universe.particles if p.size > 0]
    universe.update()

    for particle in universe.particles:
        #draw all particles on the screen
        pygame.draw.circle(screen, particle.colour, (int(particle.x), int(particle.y)), particle.size)

    for particle in universe.particles[:]:
        #create hitbox for particle collision detection with player
        particle_rect = pygame.Rect(particle.x - particle.size, particle.y - particle.size, particle.size * 2, particle.size * 2)

        if player.rect.colliderect(particle_rect):
            #reduce player lives if hit by a particle
            lives -= 1
            print(f"Lives left: {lives}")
            universe.particles.remove(particle)

            #play hit sound
            player_hit_sound.play()

            if lives == 0:
                #display game over screen
                print("Game Over!")
                screen.fill((0, 0, 0))
                game_over_text = font.render("Game Over!", True, (255, 0, 0))
                instructions_text = font.render("Press R to Restart or ESC to Quit", True, (255, 255, 255))
                game_over_score_text = font.render(f"Your Score - {score}", True, (255, 255, 255))
                #play game over sound
                game_over_sound.play()

                screen.blit(game_over_score_text, (width // 2 - 110, height // 2 - 20))
                screen.blit(game_over_text, (width // 2 - 100, height // 2 - 50))
                screen.blit(instructions_text, (width // 2 - 225, height // 2 + 20))

                pygame.display.flip()

                running = False
                waiting = True

                #handle restart or quit input
                while waiting:
                    for event in pygame.event.get():
                        if event.type == pygame.QUIT:
                            waiting = False
                            running = False
                        if event.type == pygame.KEYDOWN:
                            if event.key == pygame.K_ESCAPE:
                                waiting = False
                                running = False
                            if event.key == pygame.K_r:
                                print("Restarting Game...")
                                waiting = False
                                
                                #reset game variables
                                lives = 3
                                score = 0
                                level = 1

                                universe.particles.clear()
                                num_particles = 5

                                #generate new particles for restart
                                for _ in range(num_particles):
                                    particle_type = random.choice(["big", "medium", "small"])

                                    if particle_type == "big":
                                        size = random.randint(30, 40)
                                        mass = random.randint(500, 1500)
                                        speed = random.uniform(0.5, 2)

                                    elif particle_type == "medium":
                                        size = random.randint(20, 30)
                                        mass = random.randint(300, 800)
                                        speed = random.uniform(2, 4)

                                    else:
                                        size = random.randint(10, 20)
                                        mass = random.randint(100, 500)
                                        speed = random.uniform(3, 5)

                                    universe.addParticles(
                                        x = random.randint(50, width - 50),
                                        y = random.randint(50, height // 2),
                                        size = size,
                                        mass = mass,
                                        speed = speed,
                                        angle = random.uniform(0, 2 * 3.1415),
                                        colour = red
                                    )

                                #reset player position
                                player.rect.x = width // 2 - player_size // 2
                                player.rect.y = height - player_size - 10
                                player.bullets.clear()

                                running = True

    #display player stats
    lives_text = font.render(f"Lives - x{lives}", True, (0, 0, 0))
    score_text = font.render(f"Score: {score}", True, (0, 0, 0))
    level_text = font.render(f"Level - {level}", True, (0, 0, 0))
    screen.blit(lives_text, (10, 40))
    screen.blit(score_text, (10, 10))
    screen.blit(level_text, (10, 70))

    if lives > 0 and not waiting:
        #update player movement and render player
        player.move(keys)
        player.draw(screen)

    if len(universe.particles) == 0:
        #handle level completion
        print("Level Complete!")
        screen.fill((0, 0, 0))
        level_complete_text = font.render("Level Complete!", True, (255, 255, 255))
        instructions_text = font.render("Press Enter to Continue", True, (255, 255, 255))
        #play level complete sound
        level_complete_sound.play()

        screen.blit(level_complete_text, (width // 2 - 120, height // 2 - 50))
        screen.blit(instructions_text, (width // 2 - 150, height // 2 + 20))

        level += 1
        lives += 1  #add extra life on level completion game really hard in later levels

        pygame.display.flip()

        waiting = True
        while waiting:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    waiting = False
                    running = False
                if event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
                    waiting = False

        #reset player position for new level
        player.rect.x = width // 2 - player_size // 2
        player.rect.y = height - player_size - 10
        player.bullets.clear()

        #generate more particles for next level
        num_particles += 2
        for _ in range(num_particles):
            particle_type = random.choice(["big", "medium", "small"])

            if particle_type == "big":
                size = random.randint(30, 40)
                mass = random.randint(500, 1500)
                speed = random.uniform(0.5, 2)

            elif particle_type == "medium":
                size = random.randint(20, 30)
                mass = random.randint(300, 800)
                speed = random.uniform(2, 4)

            else:
                size = random.randint(10, 20)
                mass = random.randint(100, 500)
                speed = random.uniform(3, 5)

            universe.addParticles(
                x = random.randint(50, width - 50),
                y = random.randint(50, height // 2),
                size = size,
                mass = mass,
                speed = speed,
                angle = random.uniform(0, 2 * 3.1415),
                colour = red
            )

    pygame.display.flip()
    clock.tick(60)

#exit game loop
pygame.quit()