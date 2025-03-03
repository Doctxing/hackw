# hackw
Source code for hacking school water card data

## usage

```bash
./hackw -p 50 -o FFFFFF -t CUID
```

-p means you have already payed a unit but haven't used it all up.

the follwing number represent the price you want to write it in.

-o means card owner,you should write the UID
-t means the card-type, support `UID` and `CUID`.

also you can run:

```bash
./hackw --help
```

for usage.

the part of the algorithm already made public and rewrite using golang !!!