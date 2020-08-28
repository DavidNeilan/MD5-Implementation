## Introduction
This project was created as my "Theory of Algorithms" project for 2020. Theory of Algorithms is a module as part of the 
fourth year syllabus for "Computing and Software Development" at GMIT. The module is thought by Ian McLoughlin. The
problem essentially consists of writing a program in C that is capable of producing an MD5 hash digest of an input text.

---

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

## Algorithm
The MD5 algorithm produces a fixed length 128 bit output given an input message. The input message must be finite size
however there is no other restriction on the input message size; The input message can be arbitrarily long.

The input message is broken down into 512 bit `chunks` and each `chunk` is further divided into what the RFC spec[1]
refers to as `words`. Each `word` is a 32 bit value.

In the case of reading from a file I read 512 bits at a time and store the resulting data in a buffer. The buffer has 16
indices each pointing to an underlying 32 bit number(`uint32_t buf[16];`)  This allows the words of a given chunk to be
 retrieved using the their index i.e. the first message can be returned using `buf[0]`.

As I have outlined the MD5 algorithm can operate on any finite input. To achieve this the input message is padded until
its length is congruent to 448, modulo 512. In simple terms it is padded until its length is 64 bits less than a 
multiple of 512. The padding consists of a '1' bit followed by '0's until the required length is achieved. The remaining 
64 bits are filled with the length of the entire message as a 64 bit number(two `words`). My implementation does not 
load the entire message into memory instead it is read one `chunk` at time. This saves on memory and more easily allows 
very large files to be hashed. This also means padding does not occur on the entire message as a whole but rather on the
final chunk once it has been read.

Once each `chunk` is read(and padded in the case of the ultimate `chunk`) it can be processed. The MD5 spec[1] outlines 
the process very clearly. The processing consists of four round where each round consists of sixteen operations. These 
functions use four variables: `A`, `B`, `C`, and `D`. These variables are initialised using the following values from
the RFC spec[1]:
`A = 0x67452301; B = 0xefcdab89; C = 0x98badcfe; D = 0x10325476;` 
These numbers are only used for initialisation. Each subsequent round will continue to mutate these values each round.

The final hash or message digest is formed by concatenating `A`, `B`, `C`, and `D`. In my program I express this digest 
as a 32 character long hex string as is typical of other MD5 utilities such as `md5sum`[2]

---

## Complexity


---

## References
[1] Rivest, R., 1992. RFC1321: The MD5 message-digest algorithm.

[2] `md5sum` as part of GNU core utilities

[3] Stallman, R.M., 1988. Using the GNU Compiler Collection. For GCC version, 4(2).

[4] Handbook of Applied Cryptography, by A. Menezes, P. van Oorschot, and S. Vanstone, CRC Press, 1996.

[5] GNU LibC http://www.gnu.org/software/libc/

[6] C For Dummies Blog https://c-for-dummies.com/blog/?p=1112