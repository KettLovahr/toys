# C Toys

C is a fun little language to play around with. This is a collection of toy projects that may or may not be expanded in the future.

## Compiling and running

To make cross-compilation painless, I'm using clang here. I wrote a little script that makes building these as simple as possible.

```bash
./build.sh run <project>
./build.sh run-windows <project>
./build.sh build <project>
./build.sh build-windows <project>
./build.sh build-all <project>
```

`<project>` is any .c file here, extension excluded. Example: `./build.sh run branch`

## Online Versions

I have published some of these to my website, they run on your browser, so you don't need to compile them yourself.

- [branch](https://racc.at/games/branch)
- [tree](https://racc.at/games/tree)
