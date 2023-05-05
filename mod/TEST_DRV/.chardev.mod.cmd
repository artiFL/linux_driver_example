cmd_/home/kali/Desktop/mod/TEST_DRV/chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("/home/kali/Desktop/mod/TEST_DRV/"$$0) }' > /home/kali/Desktop/mod/TEST_DRV/chardev.mod
