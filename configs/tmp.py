with open('mass.txt') as f:
    content = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
lines = [x.strip().replace('mumu','MuMu').replace('ee','EE') for x in content]

for l in sorted(lines, key=lambda s: s.split(' ')[2]):
    s = l.split(' ')
    if 'EE' in l:
        print s[0],s[2],int(float(s[3])),int(float(s[4]))
for l in sorted(lines, key=lambda s: s.split(' ')[2]):
    s = l.split(' ')
    if 'MuMu' in l:
        print s[0],s[2],int(float(s[3])),int(float(s[4]))
