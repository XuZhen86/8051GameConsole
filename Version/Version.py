#!/Library/Frameworks/Python.framework/Versions/3.7/bin/python3

# Read major,minor,patch,build from file
file=open('Version/VersionNumbers.txt','r')
major,minor,patch,build=[int(val) for val in file.readlines()]
file.close()

# Advance build and Print version on screen
build+=1
print('Version {:d}.{:d}.{:d}.{:d}'.format(major,minor,patch,build))

# Write major,minor,patch,build to file
file=open('Version/VersionNumbers.txt','w')
file.write('{:d}\n{:d}\n{:d}\n{:d}\n'.format(major,minor,patch,build))
file.close()

# Generate header file containing version info to be compiled
file=open('Source/Version/VersionNumber.h','w')
file.write('#ifndef VERSIONNUMBER_H_\n')
file.write('#define VERSIONNUMBER_H_\n')
file.write('\n');
file.write('#define VERSION_STRING "{:d}.{:d}.{:d}.{:d}"\n'.format(major,minor,patch,build))
file.write('\n');
file.write('enum VERSION_NUMBER{\n')
file.write('    VERSION_MAJOR={:d},\n'.format(major))
file.write('    VERSION_MINOR={:d},\n'.format(minor))
file.write('    VERSION_PATCH={:d},\n'.format(patch))
file.write('    VERSION_BUILD={:d},\n'.format(build))
file.write('};\n')
file.write('\n')
file.write('#endif')
file.write('\n')
file.close()
