make &&
mkisofs -b boot/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table \
   -iso-level 3 -r -J \
   -publisher "FreeOS" \
   -o ./bin/image.iso  ./bin/cdrom/ &&
qemu -boot d -m 32 -cdrom  './bin/image.iso' -net nic,vlan=0 -net user,vlan=0 -localtime 

