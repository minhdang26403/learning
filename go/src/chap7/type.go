package main

import (
	"fmt"
	"io"
	"os"
)

func main() {
	var w io.Writer
	w = os.Stdout
	f := w.(*os.File)

	fmt.Printf("%T: %v\n", f, f)

	rw := w.(io.ReadWriter)
	fmt.Printf("%T\n", rw)
	
}