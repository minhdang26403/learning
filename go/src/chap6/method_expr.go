package main

import (
	"math"
	"fmt"
)

type Point struct{ X, Y float64 }

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

type Ptr *int

func (p Point) Print() {
	fmt.Printf("The point: (%g, %g)\n", p.X, p.Y)
}

func main() {
	x := Point{1, 2}
	y := Point{3, 4}
	distance := Point.Distance
	var d2 func(p, q Point) float64

	d2 = Point.Distance
	fmt.Printf("%T\n", distance)
	fmt.Println(distance(x, y))
	fmt.Println(d2(x, y))

	var z int
	z = 2
	fmt.Printf("%T\n", z << 2)
}