def get_the_winner(N):
    if N % 2:
        return "Alice"
    else:
        return "Bob"

if __name__ == "__main__":
    N = int(input().strip())
    print(get_the_winner(N))