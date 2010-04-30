cd rev
make
cd .. 
mkisofs -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table \
   -iso-level 3 -r -J \
   -publisher "FreeOS" \
   -o ./build/image.iso  ./build/cdrom/ 
qemu -boot d -m 128 -cdrom  './build/image.iso' -net nic,vlan=0 -net user,vlan=0 -localtime 

