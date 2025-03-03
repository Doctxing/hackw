package core

import "fmt"

type Line [2]uint64

type Block struct {
	data [3]Line
	key  Line
}

type Area struct {
	Block [16]Block
}

var usualKey = Line{0xffffffffffffff07, 0x8069ffffffffffff}

func Calculate8Key(id *[4]byte, key *[6]byte) {
	key[0] = id[0] - 0x3d
	key[1] = id[1] - 0x0f
	key[2] = id[2] | 0x39
	key[3] = (id[3] - 0xdb) ^ 0x29
	key[4] = (id[0] + 0x11) ^ 0x65
	key[5] = (id[3] + 0x30) & 0x77
}

// Generate0Data typ 0:UID,1:CUID,FUID,UFUID
func Generate0Data(id *[4]byte, typ bool) Block {
	head := Line{}
	for i := 0; i < 4; i++ {
		head[0] |= uint64(id[i]) << (56 - 8*i)
	}
	head[0] |= uint64(id[0]^id[1]^id[2]^id[3]) & 0x00ff << 24
	if typ {
		head[0] |= 0x0804 << 8
	}
	return Block{[3]Line{head, {}, {}}, usualKey}
}

func GenerateUData() Block {
	return Block{[3]Line{{}, {}, {}}, usualKey}
}

func Generate8Data(id *[4]byte, price float64, times byte, half bool) Block {
	head := Line{}
	data := Line{}
	keyLine := usualKey
	head[0] = uint64(id[0])<<56 | uint64(id[1])<<48 | uint64(id[2])<<40 | 0x01<<32
	head[1] = uint64(id[0]+id[1]+id[2]+0x01) & 0x00ff
	if price > 163.83 {
		fmt.Println("price is too big, high bits cleared")
	}
	p := int16(price*100) % 16384
	data[0] = uint64(p%256)<<56 | uint64(p/256)<<48
	check := p + p>>8 + int16(times)
	if half {
		data[0] |= 0x05 << 16
		check += 0x05
	}
	data[1] = uint64(times)<<24 | uint64(check&0x00ff)
	var key = [6]byte{}
	Calculate8Key(id, &key)
	keyLine[0] &= 0x000000000000ffff
	keyLine[1] &= 0xffff000000000000
	for i := 0; i < 6; i++ {
		keyLine[0] |= uint64(key[i]) << (56 - 8*i)
		keyLine[1] |= uint64(key[i]) << (40 - 8*i)
	}
	return Block{[3]Line{head, data, data}, keyLine}
}
