#! /bin/sh

# Done by Yuri Gouzevitch
# ygouzevi@cern.ch
# 2011/10/28

function usage
{
    echo "Usage : <Filepath> <nsDirectory> [<printGood> [<outputfile>]]"
    echo "      - Filepath : path to the ns server."
    echo "      - nsDirectory : path to the ns directory where root files are stored."
    echo "      - printGood : which files will be printed on cout. Values :'printGood' or 'printBad'."
    echo "      - outputfile : Files will not be printed on cout but in a file. if printGood, the file will be formated well in a cms.untracked.vstring."

    echo ""
    echo "Exemple of use : ./massNsCheckFileValidity.sh $CASTOR_HOME/ FINAL_MC_PT20/ printGood tmp.py"
    exit 1
}

##################
## Options reading
##################

if [[ ($# -ne 2) && ($# -ne 3) && ($# -ne 4) ]]
then 
    echo "To much or not enough args."
    usage
fi

path=$1		# something like : /castor/cern.ch/user/y/ygouzevi/
dirname=$path/$2	# directory path on ns

# Here we define which file we will printed
printGood=1
if [[ (($# -eq 3) || ($# -eq 4)) ]]
then
    if [[ $3 == "printGood" ]]  
    then
	printGood=1
    else
	if [[ $3 == "printBad" ]]  
	then
	    printGood=0
	else
	    echo "Wrong third option."
	    usage
	fi
    fi
fi

if [[ $printGood -eq 1 ]]
then
    echo "I will print only GOOD files"
else  
    echo "I will print only BAD files"
fi

printInFile=0
if [[ ($# -eq 4) ]]
then
    outputFile=$4
    printInFile=1
fi

#############
## processing
#############

if [[ ($printGood -eq 1) && ($printInFile -eq 1) ]]
then
    if [[ -e $outputFile ]]
    then
	echo "Output file : $outputFile allready exists." 
	exit 1
    fi

    touch $outputFile

    echo "import FWCore.ParameterSet.Config as cms" >>$outputFile
    echo "" >>$outputFile
    echo "fileName=cms.untracked.vstring()" >>$outputFile
    echo "" >>$outputFile
    echo "source = cms.Source(
	\"PoolSource\", 
	noEventSort = cms.untracked.bool(True), 
	duplicateCheckMode = cms.untracked.string(\"noDuplicateCheck\"), 
	fileNames = fileName
        )" >>$outputFile

    echo "" >>$outputFile
    echo "fileName.extend([" >>$outputFile 
fi

rm -rf ___tmp_file_l2
nsls $dirname >___tmp_file_l2 
size=`cat ___tmp_file_l2| wc -l`
echo "File number : $size"

count=0

for i in `seq 1 $size`
do
    f1=`head -n $i ___tmp_file_l2 | tail -n 1`
    edmFileUtil "rfio:$dirname/$f1" >/dev/null 1>/dev/null
    exitVal=$?

    if [[ $printGood -eq 1 ]]
    then 
        if [[ $exitVal -eq 0 ]]
	then
	    if [[ $printInFile -eq 0 ]]
	    then
		echo $f1
	    else
		count=$(( $count + 1 ))
		if [[ $count -lt 200 ]]
		then
		    echo $f1
		    echo "    \"rfio:$dirname/$f1\"," >>$outputFile
		else
		    count=0
		    echo "    \"rfio:$dirname/$f1\"" >>$outputFile
		    echo "])" >>$outputFile
		    echo "" >>$outputFile 
		    echo "fileName.extend([" >>$outputFile

		    echo "Spliting of the extend happend !"
		    echo $f1
		fi
	    fi
	fi
    else
        if [[ $exitVal -ne 0 ]]
	then
	    if [[ $$printInFile -eq 0 ]]
	    then
		echo $f1
	    else
		echo $f1 >>$outputFile
	    fi
	fi
    fi
done

if [[ ($printGood -eq 1) && ($printInFile -eq 1) ]]
then
    echo "])" >>$outputFile
fi

rm ___tmp_file_l2
