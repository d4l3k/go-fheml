package gobrain

import (
	"log"
	"math"
	"math/rand"

	"github.com/d4l3k/go-fheml/seal"
)

func (nn *FeedForward) random(a, b float64) *seal.Ciphertext {
	return nn.Encryptor.Encrypt(nn.Encoder.Encode((b-a)*rand.Float64() + a))
}

func (nn *FeedForward) matrix(I, J int) [][]*seal.Ciphertext {
	c := nn.Encryptor.Encrypt(nn.Encoder.Encode(0))
	m := make([][]*seal.Ciphertext, I)
	for i := 0; i < I; i++ {
		m[i] = make([]*seal.Ciphertext, J)
		for j := 0; j < J; j++ {
			m[i][j] = c.Copy()
		}
	}
	return m
}

func (nn *FeedForward) vector(I int, fill float64) []*seal.Ciphertext {
	c := nn.Encryptor.Encrypt(nn.Encoder.Encode(fill))
	v := make([]*seal.Ciphertext, I)
	for i := 0; i < I; i++ {
		v[i] = c.Copy()
	}
	return v
}

func (nn *FeedForward) sigmoid(x *seal.Ciphertext) *seal.Ciphertext {
	x = x.Copy()
	nn.Evaluator.RelinearizeInplace(x, nn.RelinKeys)
	for x.Scale() > math.Pow(2, 64) {
		nn.Evaluator.RescaleToNextInplace(x)
	}
	log.Println(x.Scale())
	nn.Evaluator.SquareInplace(x)
	nn.Evaluator.RelinearizeInplace(x, nn.RelinKeys)
	return x
	//return 1 / (1 + math.Exp(-x))
}

func (nn *FeedForward) dsigmoid(y *seal.Ciphertext) *seal.Ciphertext {
	c := nn.Encryptor.Encrypt(nn.Encoder.EncodeParmsIDScale(1, y.ParmsID(), y.Scale()))
	nn.Evaluator.SubInplace(c, y)
	nn.Evaluator.MultiplyInplace(c, y)
	return c
}
