// reverse an array
package main

import "fmt"

// reverse reverses a slice of ints in place
func reverse(s []int) {
	for i, j := 0, len(s) - 1; i < j; i, j = i + 1, j - 1 {
		s[i], s[j] = s[j], s[i]
	}
}

func reversePtr(s *[6]int) {
	n := len(s)
	for i := 0; i < n / 2; i++ {
		s[i], s[n - 1 - i] = s[n - 1 - i], s[i]
	}
}

func main() {
	// s := []int{0, 1, 2, 3, 4, 5}
	// reverse(s[:2])
	// fmt.Println(s)
	// reverse(s[2:])
	// fmt.Println(s)
	// reverse(s)
	// fmt.Println(s)

	s := [6]int{0, 1, 2, 3, 4, 5}
	reversePtr(&s)
	fmt.Println(s)
}