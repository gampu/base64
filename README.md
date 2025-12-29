`base64` is a command line tool to encode string data to and from base64 format.


```
./base64

base64 <operation> <representation> [input]

operation:       -e  Encode input to base64 format
                 -d  Decode input from base64 format

representation:  -a  Use ascii representation for IO
                 -b  Use binary representation for IO

input:               For encoding: Any valid ascii/
                     binary data   
                     For decoding: Any valid base64
                     data
                     If left empty, would wait for
                     input from terminal
```

```
Encode "hello" to base64.
./base64 -e -a hello
aGVsbG8=
```

```
Decode "aGVsbG8=" to ASCII.
./base64 -d -a aGVsbG8=
hello
```
```
Encode "abc" in ASCII form to base64.
./base64 -e -a abc
YWJj

Encode "abc" in binary form to base64.
./base64 -e -b 011000010110001001100011 # That's "abc" in binary.
YWJj

Decode "YWJj" to ASCII form.
./base64 -d -a YWJj
abc

Decode "YWJj" to binary form.
./base64 -d -b YWJj
011000010110001001100011 # That's "abc" in binary.
```

Note: When encoding, the representation flag denotes the format of the input whereas when decoding, it denotes the format of the output.
