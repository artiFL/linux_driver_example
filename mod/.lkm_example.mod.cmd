cmd_/home/kali/Desktop/mod/lkm_example.mod := printf '%s\n'   lkm_example.o | awk '!x[$$0]++ { print("/home/kali/Desktop/mod/"$$0) }' > /home/kali/Desktop/mod/lkm_example.mod
