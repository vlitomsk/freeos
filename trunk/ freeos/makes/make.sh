cd OS
make 
cd .. 
mkisofs -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table \
   -iso-level 3 -r -J \
   -publisher "OS_k-16" \
   -o image.iso  ./cdrom/ 
qemu -boot d -m 128 -cdrom  './image.iso' -net nic,vlan=0 -net user,vlan=0 -localtime 

