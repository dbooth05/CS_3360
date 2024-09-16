main: main.o
	g++ main.o -o main

main.o: main.cpp camera.h color.h hittable_list.h hittable.h interval.h ray.h rtweekend.h sphere.h vec3.h
	g++ -c main.cpp

clean:
	rm -rf main main.o image.ppm