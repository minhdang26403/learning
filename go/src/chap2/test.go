// Program to test random theories
package main

import "fmt"

type A struct{}

func main() {
	// var a int = 1
	// var b int64 = 1
	// fmt.Printf("%v\n", a == b)
	s := "你好"
	for i := 0; i < len(s); i++ {
		fmt.Printf("%c\n", s[i])
	}
	fmt.Println("----------")
	for _, c := range s {
		fmt.Printf("%c\n", c)
	}
}