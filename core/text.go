package core

import (
	"fmt"
)

func PrintHelp() {
	fmt.Printf("Usage: hackw \033[32m[PRICE] \033[0m{[OPTIONS] [DATA]}... \n\033[31mHack the water card data.\033[0m\nLater you need to flash the '.bin' or '.dump' file to a new card.\n\n")
	fmt.Printf("Mandatory arguments to long options are mandatory for short options too\n")
	fmt.Printf("\nOptions:\n")
	fmt.Printf("  -p, --partly-used\twhen you pay a unit but haven't\n")
	fmt.Printf("\t\t\tused it all up\n")
	fmt.Printf("  -c, --usage-count\tto make the data more likely a\n")
	fmt.Printf("\t\t\tnormal one?\n")
	fmt.Printf("  -t, --card-type\tset the type of the card. such as\n")
	fmt.Printf("\t\t\tUID \n\t\t\tCUID\033[33m(Default)\033[0m\n")
	fmt.Printf("  -o, --owner-data\tuse advanced mode, write the user\n")
	fmt.Printf("\t\t\tdata into the binary file. \033[32mor the\n")
	fmt.Printf("\t\t\tprogram will use random UIDs to p-\n")
	fmt.Printf("\t\t\trotect our door from getting hacked\033[0m\n")
	fmt.Printf("      --help\t\tprint the prompts\n")
}
