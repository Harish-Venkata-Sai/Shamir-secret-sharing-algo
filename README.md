# 🔐 Shamir's Secret Sharing - C++ Implementation

This project demonstrates a robust C++ implementation of **Shamir's Secret Sharing** with support for:

- Invalid share detection
- Arbitrary base decoding
- Large integer handling using GMP
- Secret reconstruction via Lagrange interpolation

---

## 📁 Project Structure

```
/Hashira
├── main.cpp              # Main implementation
├── testcase1.json        # Sample test case 1
├── testcase2.json        # Sample test case 2
└── README.md             # Documentation
```

---

## 🎯 Problem Statement

You are given a set of `n` shares, out of which at least `k` are needed to reconstruct the original secret. Some shares may be invalid (tampered or corrupted). Your task is to:

- Parse the share data from JSON
- Identify and ignore invalid shares
- Reconstruct the original secret using valid combinations of `k` shares

This mimics real-world cryptographic scenarios like **threshold-based access**, **secure key recovery**, or **multi-party authorization** in **blockchain systems**.

---

## 🧪 JSON Input Format

Each test case JSON follows this format:

```json
{
  "keys": { "n": 5, "k": 3 },
  "1": { "base": "10", "value": "12345" },
  "2": { "base": "16", "value": "1A2B3C" },
  ...
}
```

- `"k"`: Minimum number of valid shares required
- Each key (`"1"`, `"2"`, etc.): A share with its base and encoded value

---

## 🛠️ Tech Stack

- **C++17**
- **GMP**: For handling large numbers (20–40 digits and beyond)
- **nlohmann/json**: For elegant JSON parsing
- **Lagrange Interpolation**: For reconstructing the constant term (secret)

---

## 🚀 How to Run

### Step 1: Install Dependencies

#### On Ubuntu/Debian

```bash
sudo apt install libgmp-dev g++ cmake
```

#### On macOS (with Homebrew)

```bash
brew install gmp
```

### Step 2: Compile

```bash
g++ -o secret main.cpp -lgmp -lgmpxx
```

### Step 3: Run

```bash
./secret
```

You’ll see output like:

```
Secret for file testcase1.json: 2
Invalid shares for file testcase1.json: None

Secret for file testcase2.json: 79836264049850
Invalid shares for file testcase2.json: 8 
```

---

## 🧠 How It Works

1. **Base Decoding**: Converts values from their respective bases to integers.
2. **Share Combination**: Tries all possible combinations of `k` shares.
3. **Lagrange Interpolation**: Computes the polynomial's constant term (`f(0)`), which is the original secret.
4. **Majority Voting**: Among all combinations, the most frequent secret is chosen.
5. **Invalid Share Detection**: Remaining shares that never contributed to valid reconstructions are flagged as invalid.

---

## 💡 Example Enhancement Ideas

- Add support for finite field arithmetic with a prime modulus (`mod p`)
- Display invalid share indices
- Show reconstructed polynomial (if extended to full polynomial)
- Add logging/visualizations of share combinations

---

## 📜 Related Concepts

- [Shamir's Secret Sharing (Wikipedia)](https://en.wikipedia.org/wiki/Shamir%27s_Secret_Sharing)
- [GMP - GNU Multi-Precision Library](https://gmplib.org/)
- Cryptography in distributed systems, blockchain, and threshold signatures