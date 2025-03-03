package main

import (
	"encoding/binary"
	"fmt"
	"hackw/core"
	"math/rand"
	"os"
	"time"
)

func main() {
	rand.Seed(time.Now().UnixNano())
	args := os.Args[1:]
	var partlyUsed, cardType bool
	var uid [4]byte
	var times byte
	var price float64

	//初始化
	for i := 0; i < 4; i++ {
		uid[i] = byte(rand.Intn(256)) // 生成 0 到 255 之间的随机整数
	}
	times = 1
	price = 50
	cardType = true

	//读取args
	for i := 0; i < len(args); i++ {
		arg := args[i]
		if arg[0] == '-' {
			if arg[1] == '-' {
				switch arg {
				case "--partly-used":
					{
						partlyUsed = true
						break
					}
				case "--usage-count":
					{
						i++
						_, err := fmt.Sscanf(args[i], "%d", &times)
						if err != nil {
							fmt.Printf("%v: %s\n", err, args[i])
							return
						}
						break
					}
				case "--card-type":
					{
						i++
						if args[i] == "UID" {
							cardType = false
						} else if args[i] == "CUID" {
							cardType = true
						} else {
							err := fmt.Errorf("invalid card type: %s", args[i])
							fmt.Println(err)
							return
						}
						break
					}
				case "--owner-data":
					{
						i++
						_, err := fmt.Sscanf(args[i], "%02x%02x%02x%02x", &uid[0], &uid[1], &uid[2], &uid[3])
						if err != nil {
							fmt.Println("not a 4-byte HEX ID:", args[i])
							return
						}
						break
					}
				case "--help":
					{
						core.PrintHelp()
						return
					}
				default:
					{
						err := fmt.Errorf("invalid arg: %s", arg)
						fmt.Println(err)
						return
					}
				}
			} else {
				for j := 1; j < len(arg); j++ {
					switch arg[j] {
					case 'p':
						{
							partlyUsed = true
							break
						}
					case 'c':
						{
							i++
							_, err := fmt.Sscanf(args[i], "%d", &times)
							if err != nil {
								fmt.Printf("%v: %s\n", err, args[i])
								return
							}
							break
						}
					case 't':
						{
							i++
							if args[i] == "UID" {
								cardType = false
							} else if args[i] == "CUID" {
								cardType = true
							} else {
								err := fmt.Errorf("invalid card type: %s", args[i])
								fmt.Println(err)
								return
							}
							break
						}
					case 'o':
						{
							i++
							_, err := fmt.Sscanf(args[i], "%02x%02x%02x%02x", &uid[0], &uid[1], &uid[2], &uid[3])
							if err != nil {
								fmt.Println("not a 4-byte HEX ID:", args[i])
								return
							}
							break
						}
					default:
						{
							err := fmt.Errorf("invalid arg: %s", arg)
							fmt.Println(err)
							return
						}
					}
				}
			}
		} else {
			_, err := fmt.Sscanf(arg, "%f", &price)
			if err != nil {
				fmt.Println(err)
			}
		}
	}

	//生成
	var binarydata core.Area
	block0 := core.Generate0Data(&uid, cardType)
	blockU := core.GenerateUData()
	block8 := core.Generate8Data(&uid, price, times, partlyUsed)
	binarydata.Block[0] = block0
	binarydata.Block[8] = block8
	for i := 0; i < 16; i++ {
		if i != 0 && i != 8 {
			binarydata.Block[i] = blockU
		}
	}

	// 写入文件
	if file, err := os.Create("outdata.dump"); err == nil {
		defer func(file *os.File) {
			err := file.Close()
			if err != nil {
				fmt.Println(err)
			}
		}(file)
		if err := binary.Write(file, binary.BigEndian, &binarydata); err == nil {
			fmt.Println("Done!")
			return
		}
		fmt.Println("Write error:", err)
	} else {
		fmt.Println("Can't create file:", err)
	}
}
