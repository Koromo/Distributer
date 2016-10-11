import sys
import subprocess

argv = sys.argv
argc = len(argv)

if (argc != 3):
    print "Usage: " + argv[0] + " homeworks table"
    print "homeworks: Homeworks path"
    print "table: Distribution table path."
    exit()

homeworks_path = argv[1]
table_path = argv[2]
table_file = open(table_path, "r")

marker_names = set()
for line in table_file:
    if line[0] == "-":
        tokens = line.split("\t")
        marker_names.add(tokens[0][1:])

for name in marker_names:
    subprocess.call(["mkdir", name])

table_file.seek(0)
for line in table_file:
    if line[0] != "-":
        continue

    tokens = line.split("\t")
    marker_name = tokens[0][1:]
    date = tokens[1]
    number = tokens[2][:-1]

    cp_src_path = homeworks_path + "/" + date + "/" + number
    cp_dest_path = marker_name + "/" + number
    subprocess.call(["mkdir", cp_dest_path])
    subprocess.call("cp -r " + cp_src_path + "* " + cp_dest_path, shell = True)

for name in marker_names:
    subprocess.call(["zip", "-r", name + ".zip", name])
