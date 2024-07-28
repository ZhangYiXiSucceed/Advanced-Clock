import argparse 
import os

def concatenate_files(file1, file2_dir, output_file):
    try:
        with open(file1, 'rb') as f1,open(output_file, 'wb') as fout:
            # 读取第一个文件的内容并写入输出文件
            file1_buf = f1.read()
            fout.write(file1_buf)

            source_file_size = os.path.getsize(output_file)
            print("size={}".format(source_file_size))
            
            padding_size = 0x10000 - source_file_size
            padding_buf = ""
            for i in range(padding_size):
                padding_buf += chr(0)
            padding_buf = bytes(padding_buf, "utf-8")
            print("size={}".format(len(padding_buf)))
            fout.write(padding_buf)

            print("size={}".format(os.path.getsize(output_file)))

            for i in range(3):
                file_name = file2_dir + "/ER_OVERLAY" + str(i)
                with open(file_name, "rb") as f:
                    # 读取第二个文件的内容并写入输出文件
                    file_buf = f.read()
                    fout.write(file_buf)

                    adding_size = 0x400 - os.path.getsize(file_name)
                    padding_buf = ""
                    for i in range(adding_size):
                        padding_buf += chr(0)
                    padding_buf = bytes(padding_buf, "utf-8")
                    print("size={}".format(len(padding_buf)))
                    fout.write(padding_buf)

            
            print(f"Files {file1} and overlay file have been concatenated into {output_file}")
    except IOError as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    # 示例用法
    parser = argparse.ArgumentParser(description='Calculate the sum or product of two integers.')
      # 添加位置参数
    parser.add_argument('file_dir', type=str, help='bin file dir')

    # 解析命令行参数
    args = parser.parse_args()
    main_file = args.file_dir + "/ER_IROM1"
    bank_file = args.file_dir 
    output_file = "./out/AdvancedClock.bin"
    concatenate_files(main_file, bank_file, output_file)