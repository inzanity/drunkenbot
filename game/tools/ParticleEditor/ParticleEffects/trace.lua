Options = {}
Options.Particles = 150
Options.Coordinates = 20
Options.Colors = 1
Options.Sizes = 1
Options.Duration = 2000
Options.Looping = true
Options.Texture = "../particles/64x64_default.bmp"
initParticleSystem(Options)
size = 5
life = 2000
pos = {}
for i = 1, Options.Particles do
	math.randomseed(767) -- test other seeds too =)
	for j = 1, Options.Coordinates do
		pos[j] = {}
		pos[j].x = math.random(-size, size)
		pos[j].y = math.random(-size, size)
		pos[j].z = math.random(-size, size)
	end
	f = 1 - (i - 1) / Options.Particles
	color = {rgb(50*f, 50*f, 255*f)}
	setParticle(i, life, -i*2, pos, color)
end
sizes = {.2}
setDefaultSize(sizes)