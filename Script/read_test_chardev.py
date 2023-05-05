for x in range(11):
    f = open("/dev/chardev", "wb+", buffering=0)
    print(f.read())
f.close()




