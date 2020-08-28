# MD5 Hashing Algorithm Implementation
 This repository is intended to provide an implementation of the MD5 specification as outlined in
 [RFC 1321](https://www.ietf.org/rfc/rfc1321.txt). This project should be suitable to use as a teaching aide to explain
 the algorithm to software development students or those with a basic understanding of programming and computer science
 concepts. This MD5 implementation is written is C. A good working of knowledge of C will make the code easier to follow
 although it is not strictly required as each section should be explain and design choices and implementation details.
 
 ## Run
 To run this program you will need a C compiler. All source code in this repository has been wrote and tested against
 `Solus x86_64` using `gcc`[3] as its compiler. However the code should be platform independent. Below is a brief guide
 on how to install the required dependencies on some populate Linux platforms.
 
 ### Prerequisites
 * #### Solus
  ~~~
  sudo eopkg install -c system.devel git
  ~~~
 
 * #### Ubuntu 20.04 LTS
  ~~~
  sudo apt install gcc git
  ~~~
 
 * #### Fedora 22+
  ~~~
  sudo dnf groupinstall 'Development Tools'
  sudo dnf install git
  ~~~
 
 * #### Arch
  ~~~
  sudo pacman -Syu gcc git
  ~~~
 
 ### Complie & Run
 1) Download the source code with `git` and complete it using `gcc`[3]
    ~~~
    git clone https://github.com/DavidNeilan/MD5-Implementation.git md5
    cd md5
    gcc -o md5 -lm src/main.c 
    ~~~
 
 2) There should now be binary in your current directory named `md5`. You can test it with the command below. You should
 expect to see the following hash returned `b10a8db164e0754105b7a99be72e3fe5`
    ~~~
    ./md5 "Hello World"
    ~~~
    
 3) Now that the correct hash has been returned you can use the application to has and string. To has a file use the `-f`
 flag.
    ~~~
    ./md5 -f [file]
    ~~~
 4) A full list of command line arguments can be found using `./md5 --help`
 
 ---
 
 ## Test
 The `md5` binary can be tested by comparing its outputs to known component_hashes. A test suite is built as part of the binary.
 This can be run using the `--test` flag. Alternatively feel free to compare the result against other known component_hashes
 manually.
 ~~~
 ./md5 --test
 ~~~
 
 `Solus` and many other popular Linux distributions  come with a MD5 utility pre-installed. The pre-installed MD5 utility
 can be used to test the newly created binary. On `Solus` the utility is called `md5sum`[2] and it can hash strings using
 the following. Note the `-n` flag on `echo` this omits the newline character that `echo` typically appends. Without
 this flag the wrong hash may be produced.
 ~~~
 echo -n "Hello World" | md5sum
 ~~~
 
 --- 