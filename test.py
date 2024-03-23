import subprocess
import json
# 定义命令列表
commands = [
    r".\PreliminaryJudge.exe -f 0 -s {} -m maps\map-3.{}.txt .\cmake-build-debug\main.exe -l ERR".format(i, map_version)
    for map_version in range(7, 15)
    for i in range(0, 24)
]

print(3)
# 创建一个空列表来存储输出结果
cnt = 7
with open("log6.txt", "w") as log_file:
    output_results = []
    # 循环执行每个命令
    for idx, command in enumerate(commands):
        # 使用subprocess.Popen执行命令，并捕获标准输出和标准错误
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()

        try:
            stdout = stdout.decode('utf-8')
        except UnicodeDecodeError:
            stdout = stdout.decode('gbk')

        try:
            stderr = stderr.decode('utf-8')
        except UnicodeDecodeError:
            stderr = stderr.decode('gbk')

        # 将输出结果添加到output_results列表中
        idx = idx % 24
        if idx == 0:
            print("map: ", cnt)
            log_file.write("map-3." + str(cnt) + "\n")
            cnt += 1
        print("seed: ", idx)
        print("标准输出：", stdout)
        try:
            data = json.loads(stdout)
        except json.JSONDecodeError:
            print("json error")
            continue
        output_results.append(data["score"])
        # print("标准错误：", stderr)
        log_file.write("seed:" + str(idx) + "\n")
        log_file.write("result:" + stdout + "\n")
        # log_file.write("标准错误：\n" + stderr + "\n")
        count = 0
        if idx == 23:
            for i in output_results:
                log_file.write(str(i) + "\n")
                count += int(i)

            log_file.write("tot：" + str(count/24) + "\n")
            output_results = []


