from subprocess import Popen, PIPE

# Define the dataset class with all the properties
class dataset:
    def __init__(self):
        self.dt = None
        self.d = None
        self.xsec = None
        self.xsecerr = None
        self.nevs = None
        self.weight = ''
        self.cmsRun = ''

f=open('datasets_80X_tmp.dat')
g=open('datasets_80X_tmp2.dat','w')

for line in f:
    if '#' in line:
        g.write(line)
        continue

    ds = dataset();
    ds.dt=line.strip().split('\t')[0]
    ds.d=line.strip().split('\t')[1]
    ds.xsec = line.strip().split('\t')[2]
    ds.xsecerr = line.strip().split('\t')[3]
    ds.nevs = line.strip().split('\t')[4]
    ds.weight = line.strip().split('\t')[7]

    if 'Run' in ds.dt:
        g.write(line)
        continue
    if 'TTJets' in ds.dt:
        g.write(line)
        continue
    if 'DYJets' in ds.dt:
        g.write(line)
        continue

    print ds.dt
    h=open('xsec.txt')
    for xsec in h:
        if ds.d.split('/')[1] in xsec:
            process = Popen(['cmsRun','../ana.py','inputFiles='+xsec], stdout=PIPE, stderr=PIPE)
            stdout, stderr = process.communicate()
            xline = [x for x in stderr.split('\n') if 'After filter: final cross section ' in x][0]
            print xline.strip().split(' ')[6]
            ds.xsec = xline.strip().split(' ')[6]
            ds.xsecerr = xline.strip().split(' ')[8]

    g.write(ds.dt+'\t'+ds.d+'\t'+ds.xsec+'\t'+ds.xsecerr+'\t'+ds.nevs+'\troot:\t-\t'+ds.weight+'\n')
