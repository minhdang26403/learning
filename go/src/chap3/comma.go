package main

import (
	"bytes"
	"fmt"
	"os"
)

func main() {
	for i := 1; i < len(os.Args); i++ {
		fmt.Printf("  %s\n", comma(os.Args[i]))
	}
}

//!+
// comma inserts commas in a non-negative decimal integer string.
// func comma(s string) string {
// 	n := len(s)
// 	if n <= 3 {
// 		return s
// 	}
// 	return comma(s[:n-3]) + "," + s[n-3:]
// }

// non-recursive function
func comma(s string) string {
	var buf bytes.Buffer
	r := len(s) % 3
	if r == 0 {
		r = 3
	}
	buf.WriteString(s[:r])
	for i := r; i < len(s); i += 3 {
		buf.WriteByte(',')
		buf.WriteString(s[i:i + 3])
	}
	
	return buf.String()
}