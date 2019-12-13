# Keep the function signature,
# but replace its body with your implementation
import os
import sys
import zlib

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()


def topo_order_commits():
    path = os.getcwd()
    os.chdir(path)
    found = False
    while path != "/" and found == False:
        print(path)
        for filename in os.listdir(path):
            print(path + "/" + filename)
            if filename == ".git":
                found = True
                break
        if found == False :
            os.chdir("..")
            path = os.getcwd()
    if path == "/":
        sys.stderr.write("Not inside a Git repository")
        exit(1)
    print(path)


    
if __name__ == '__main__':
    topo_order_commits()
