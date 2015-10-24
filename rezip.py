import sys
import subprocess

argv = sys.argv
argc = len(argv)

# Check argments
if (argc != 3):
    print "Usage: " + argv[0] + " homeworks table"
    print "homeworks: Homeworks path"
    print "table: Distribution table path."
    exit()

homeworks_path = argv[1]
table_path = argv[2]

# Open table file
table_file = open(table_path, "r")
if (table_file.readline() != "SUCCEEDED\n"):
    print "Failed to rezip."
    exit()

# Create directories for each markers into output directory
marker_names = set()
for line in table_file:
    if line[0] != "-":
        continue
    splitted_line = line.split("\t")
    marker_names.add(splitted_line[0][1:])

for name in marker_names:
    subprocess.call(["mkdir", name])

# Unzip homework files into each marker directories
table_file.seek(0)
for line in table_file:
    if line[0] != "-":
        continue

    splitted_line = line.split("\t")
    marker_name = splitted_line[0][1:]
    date = splitted_line[1]
    number = splitted_line[2][:-1]

    # Create destination directory
    src_path = homeworks_path + "/" + date + "/" + number
    dest_path = marker_name + "/" + number
    subprocess.call(["mkdir", dest_path])

    # Try unzip
    ret = subprocess.call(["unzip", "-d", dest_path, src_path + ".zip"])

    # Try copy file that has number in filename if failed to unzip
    if ret != 0:
        print "Try copy."
        subprocess.call("cp " + src_path + "* " + dest_path, shell = True)

# Zip each marker directories
for name in marker_names:
    subprocess.call(["zip", "-r", name + ".zip", name])
