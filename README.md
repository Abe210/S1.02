# sae02
SAE 02 C Program


To minimize collision, for permutation names like "abcd", "bcda", "dcab" which will return the hash key, we define a constant. We chose a prime number = 31 which we will multily every character ASCII (nowadays known as UNICODE). This way, the hash output is affected by the position of the character and hash("abcd") & hash("bcde") will be different.

Optimization:
    To reduce collision and %Table giving too many zero value, we can choose an odd sized table or table size equal a prime number. (Reference Maths Discrètes on ring)

