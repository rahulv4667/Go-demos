package main

type Structure struct {
	ret_val int
}

func function_name(a, b int, c string, d float32, e []byte) (int, Structure, string, error) {
	var x int = a + b
	y := x * 2
	d = d + float32(y)
	c = c + string(int(d))
	c = c + string(e)
	return int(d), Structure{ret_val: a}, c, nil
}
