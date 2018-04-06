# cli
An optional client for BTS

It can't work alone.

If you want to use this in bitshares.
Do this things
```
git clone https://github.com/bitshares/bitshares-core.git
cd bitshares-core/libraries
echo "\nadd_subdirectory( cli )" >> CMakeLists.txt
git clone https://github.com/chouheiwa/cli.git
```
Then run as normal how to build Bitshares
