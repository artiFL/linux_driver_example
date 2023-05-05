cmd_/home/kali/Desktop/mod/Module.symvers :=  sed 's/ko$$/o/'  /home/kali/Desktop/mod/modules.order | scripts/mod/modpost -m     -o /home/kali/Desktop/mod/Module.symvers -e -i Module.symvers -T - 
