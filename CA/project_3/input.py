import subprocess

def run_command(command) :
    result = subprocess.run(command, shell=True, capture_output=True, text= True)
    return result.stdout


num_1 = [1,2,4,8,16,32,64,128,256]
num_2 = [256,128,64,32,16,8,4,2,1]

cmd_list = ["CCa", "CCe", "CCh", "CCh_st", "CCl", "CCm", 
            "CF1", "CRd", "CRf", "CRm", "CS1", "CS3", "DP1d", "DP1f", 
            "DPcvt", "DPT", "DPTd", "ED1", "EF", "EI","EM1", "EM5", 
            "M_Dyn", "MC", "MCS", "MD", "MI", "MIM", "MIM2", "MIP", "ML2", 
            "ML2_BW_ld", "ML2_BW_ldst", "ML2_BW_st", "ML2_st", "MM","MM_st", 
            "STc", "STL2", "STL2b"]
for cmd in cmd_list :
    ans_list = []
    for i in range(9) :
        command = "./spike --dc=" + str(num_1[i]) +\
                ":" + str(num_2[i]) + ":64 ~swe3005/2024s/proj3/pk ~swe3005/2024s/proj3/bench/"+\
                cmd+".elf"
        stdout = run_command(command)
        # print(stdout)
        ans_list.append(float(stdout[-8:-2]))
    index = ans_list.index(min(ans_list))
    print(f"{cmd} --dc={num_1[index]}:{num_2[index]}:64 {min(ans_list)}%")