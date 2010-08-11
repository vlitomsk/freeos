make &&
mkisofs -b boot/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table \
   -iso-level 3 -r -J \
   -publisher "FreeOS" \
   -o ./bin/image.iso  ./bin/cdrom/ &&
bochs -f bin/bochs.rc
