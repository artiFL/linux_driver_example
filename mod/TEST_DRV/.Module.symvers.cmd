cmd_/home/kali/Desktop/mod/TEST_DRV/Module.symvers :=  sed 's/ko$$/o/'  /home/kali/Desktop/mod/TEST_DRV/modules.order | scripts/mod/modpost -m     -o /home/kali/Desktop/mod/TEST_DRV/Module.symvers -e -i Module.symvers -T - 