Options = {}
Options.Particles = 200
Options.Coordinates = 3
Options.Colors = 3
Options.Sizes = 1
Options.Duration = 5000
Options.Looping = true
Options.Texture = "../particles/32x32_default.bmp"
initParticleSystem(Options)
height = 8
width = 3.5
life = 2000
pos = {}
for i = 1, Options.Particles do
	pos[1] = {x = 0, y = 0, z = 0}
	pos[2] = {}
	pos[2].x = math.random(-width, width)
	pos[2].y = math.random(height/2, height)
	pos[2].z = math.random(-width, width)
	pos[3] = {x = pos[2].x * 2, y = pos[2].y * -1, z = pos[2].z * 2}
	setParticle(i, life, i / Options.Particles * life, pos)
end
colors = {rgb(255, 0, 0), rgb(0, 0, 255), rgb(0, 0, 0)} -- todo Replace last one with rgba(0, 0, 255, 0) after blending is correct
setDefaultColor(colors)
sizes = {.2}
setDefaultSize(sizes)