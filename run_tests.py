import pandokia.helpers.process as process
import filecmp
import os

testFiles = [
    'anonymousTypes.h',
    'basicFunctions.h',
    'basicTypedef.h',
    'enums.h',
    'macros.h',
    'structures.h',
    'test3.h'
    ]


for file in testFiles:
    # run the program, capturing output into a file
    status = process.run_process( [ './dumpSparrow.out', 'tests/' + file ], output_file='testFile.tmp' )
    fileName = file.rsplit( ".", 1 )[0]
    result = filecmp.cmp('testFile.tmp', 'tests/results/' + fileName + '.spr')

    if result == False:
        print "FAIL --> " + file
    else:
        print "PASS --> " + file

    # read the file and print it to the stdout being used by the test
    # process.cat(["testFile.tmp" ])

os.remove("testFile.tmp")
