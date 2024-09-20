# C Toys

C is a fun little language to play around with. This is a collection of toy projects that may or may not be expanded in the future.

To make cross-compilation painless, I'm using clang here. I wrote a little script that makes building these as simple as possible.

```bash
./build.sh run <project>
./build.sh run-windows <project>
./build.sh build <project>
./build.sh build-windows <project>
./build.sh build-all <project>
```

`<project>` is any .c file here, extension excluded. Example: `./build.sh run branch`
