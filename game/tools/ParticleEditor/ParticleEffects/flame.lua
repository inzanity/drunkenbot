Options = {}
Options.Particles = 200
Options.Coordinates = 3
Options.Colors = 5
Options.Sizes = 3
Options.Duration = 10000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
height = 8
width = 3.5
life = 2000
pos = {}
for i = 1, Options.Particles do
	pos[1] = {x = math.random(-width, width)/5, y = 0, z = math.random(-width, width)/5}
	pos[2] = {}
	pos[2].x = math.random(-width, width)/3
	pos[2].y = height / 3
	pos[2].z = math.random(-width, width)/3
	pos[3] = {x = math.random(-width, width)/4, y = height, z = math.random(-width, width)/4}
	setParticle(i, life, i / Options.Particles * life, pos)
end
-- Last one should be rgba(255, 0, 0, 0) after blending is correct
colors = {rgb(255, 255, 0), rgb(255, 128, 0), rgb(255, 128, 0), rgb(255, 0, 0), rgb(0, 0, 0)}
setDefaultColor(colors)
sizes = {.2, .3, .1}
setDefaultSize(sizes)