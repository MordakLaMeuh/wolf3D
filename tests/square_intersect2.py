from matplotlib.pyplot import *
import numpy as np

map = """1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1"""

map = np.array([[int(n) for n in line.split()] for line in map.split("\n")], dtype=int)
print map

player_position = np.array([10.25, 6.75])
player_angle = np.linspace(0., 2.*np.pi, 1000)

def DDA(map, position, angle):
	rayPosX, rayPosY = position
	rayDirX = np.cos(angle)
	rayDirY = np.sin(angle)
	mapX = int(rayPosX)
	mapY = int(rayPosY)
	if rayDirY == 0.:
		stepX = 1 if rayDirX > 0 else -1
		while map[mapY][mapX] == 0:
			mapX += stepX
		c = np.array([mapX, mapY])
		intersect = np.array([mapX, rayPosY])
		return c, intersect
	if rayDirX == 0.:
		stepY = 1 if rayDirY > 0 else -1
		while map[mapY][mapX] == 0:
			mapY += stepY
		c = np.array([mapX, mapY])
		intersect = np.array([rayPosX, mapY])
		return c, intersect
	deltaDistX = np.sqrt(1. + rayDirY**2 / rayDirX**2)
	deltaDistY = np.sqrt(1. + rayDirX**2 / rayDirY**2)
	if (rayDirX < 0):
		stepX = -1
		sideDistX = (rayPosX - mapX) * deltaDistX
	else:
		stepX = 1
		sideDistX = (mapX + 1. - rayPosX) * deltaDistX
	if (rayDirY < 0):
		stepY = -1
		sideDistY = (rayPosY - mapY) * deltaDistY
	else:
		stepY = 1
		sideDistY = (mapY + 1. - rayPosY) * deltaDistY
	hit = 0
	while hit == 0:
		if (sideDistX < sideDistY):
			sideDistX += deltaDistX
			mapX += stepX
			side = 0
		else:
			sideDistY += deltaDistY
			mapY += stepY
			side = 1
		if map[mapY][mapX] > 0:
			hit = 1
	if side == 0:
		perpWallDist = (mapX - rayPosX + (1. - stepX) / 2.) / rayDirX
	else:
		perpWallDist = (mapY - rayPosY + (1. - stepY) / 2.) / rayDirY
	c = np.array([mapX, mapY])
	intersect = np.array([position[0] + perpWallDist * rayDirX, position[1] + perpWallDist * rayDirY])
	return c, intersect

for y in range(map.shape[0]):
	for x in range(map.shape[1]):
		if map[y][x] > 0:
			plot([x, x+1, x+1, x, x], [y, y, y+1, y+1, y], color='g')
for angle in player_angle:
	c, intersect = DDA(map, player_position, angle)
	x, y = c
	plot([x, x+1, x+1, x, x], [y, y, y+1, y+1, y], color='r')
	plot([player_position[0], intersect[0]], [player_position[1], intersect[1]], marker='x', color = 'g')
show()