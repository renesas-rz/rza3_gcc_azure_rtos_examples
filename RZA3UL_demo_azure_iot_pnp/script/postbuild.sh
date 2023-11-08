echo "Post build script started"

if [ -n "$BuildArtifactFileName" ]; then
  inputfilename=$BuildArtifactFileName
fi
if [ -n "$BUILDARTIFACTFILENAME" ]; then
  inputfilename=$BUILDARTIFACTFILENAME
fi

filebody=${inputfilename%.*}

aarch64-none-elf-objcopy -O binary ${filebody}.elf ${filebody}.bin
aarch64-none-elf-objcopy -I binary -O srec --adjust-vma=0x20020000 --srec-forceS3 ${filebody}.bin ${filebody}.srec

echo "Post build script complete"
