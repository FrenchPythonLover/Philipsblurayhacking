print("HEX2BIN_PHILIPS Utiliy\nBy FrenchPythonLover")

with open("mtd0.hex", "r") as f:
    lines = f.readlines()
print("Starting now... Will take a while")
clean_lines = [l.strip() for l in lines if len(l.strip()) == 32 and all(c in "0123456789abcdef" for c in l.strip())]
rejected = [l.strip() for l in lines if not (len(l.strip()) == 32 and all(c in "0123456789abcdef" for c in l.strip()))]

hexstr = "".join(clean_lines)
print(f"Clean lines : {len(clean_lines)}")
print(f"Trash lines : {len(rejected)}")
print(f"Rebuilt bytes : {len(hexstr)//2}")
print(f"Expected : {0x08000000}")
print("--- Rejected lines includes ---")
for r in rejected[:20]:
    print(repr(r))
