# Python implementation of the
# Sorting visualiser: Quick Sort

# Imports
import pygame
import random
import time
pygame.font.init()

# Total window
screen = pygame.display.set_mode(
			(900, 650)
		)

# Title and Icon
pygame.display.set_caption("Mo phong thuat toan sap xep!")

run = True

# Window size and some initials
width = 900
length = 600
array =[0]*151
arr_clr =[(0, 204, 102)]*151
clr_ind = 0
clr =[(0, 204, 102), (255, 0, 0),\
	(0, 0, 153), (255, 102, 0)]
fnt = pygame.font.SysFont("comicsans", 30)
fnt1 = pygame.font.SysFont("comicsans", 20)


# Function to generate new Array
def generate_arr():
	for i in range(1, 151):
		arr_clr[i]= clr[0]
		array[i]= random.randrange(1, 100)
		
# Initially generate a array
generate_arr()

# Function to refill the
# updates on the window
def refill():
	screen.fill((255, 255, 255))
	draw()
	pygame.display.update()
	pygame.time.delay(30)
	
# Sorting Algo:Quick sort
def quicksort(arr, l, r):
	x=arr[((l+r)//2)]
	arr_clr[((l+r)//2)]=clr[2]
	i=l
	j=r
	while i<j:
		while arr[i]< x:
			i+=1
			arr_clr[i]=clr[1]
		while arr[j]> x:
			j-=1
			arr_clr[j]=clr[1]
		if i<=j:
			tmp=arr[i]
			arr[i]=arr[j]
			arr[j]=tmp	
			i+=1
			j-=1
		refill()
	#pygame.event.pump()
	if l<j:
		quicksort(arr,l,j)
		refill()
	if i<r:
		quicksort(arr,i,r)
		refill()
	

def draw():
	
	txt = fnt.render("Nhan 'enter' de sap xep",\
		            1, (0, 0, 0))
	screen.blit(txt, (20, 20))
	txt1 = fnt.render("Nhan 'r'de reset mang ",\
					1, (0, 0, 0))
	screen.blit(txt1, (20, 40))
	txt2 = fnt1.render("ALGORITHM USED: QUICK SORT",\
					1, (0, 0, 0))
	screen.blit(txt2, (600, 60))
	element_width =(width-150)//150
	boundry_arr = 900 / 150
	boundry_grp = 550 / 100
	pygame.draw.line(screen, (0, 0, 0),\
				(0, 95), (900, 95), 6)
	
	# Drawing the array values as lines
	for i in range(1, 151):
		pygame.draw.line(screen,\
				arr_clr[i], (boundry_arr * i-3, 100),\
				(boundry_arr * i-3,\
				array[i]*boundry_grp + 100),\
				element_width)

while run:
	screen.fill((255, 255, 255))
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			run = False
		if event.type == pygame.KEYDOWN:
			if event.key == pygame.K_r:
				generate_arr()
			if event.key == pygame.K_RETURN:
				quicksort(array, 0, len(array)-1)	
	draw()
	pygame.display.update()

# startTime=time.time()
# quicksort(array,0,len(array)-1)
# endTime=time.time()
# print("Thoi gian chay:",endTime-startTime,"giay")
# print("mang da sap xep: ", array)
pygame.quit()
