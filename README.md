# sae02
SAE 02 C Program

The main purpose of this project is to find and implement the best data structure for easy or fast read access on a large data.

For that, we used a implemented a HashTable on C.

To minimize collision, for permutation names like "abcd", "bcda", "dcab" which will return the same hash key, we define a constant. We chose a prime number = 31 which we will multily every character ASCII (nowadays known as UNICODE). This way, the hash output is affected by the position of the character and hash("abcd") & hash("bcde") will be different.

Optimization:
    To reduce collision and %Table giving too many zero value, we can choose an odd sized table or table size equal a prime number. (Reference Maths Discrètes on ring)

