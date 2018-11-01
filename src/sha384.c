/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtimoshy <dtimoshy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 16:45:32 by dtimoshy          #+#    #+#             */
/*   Updated: 2018/10/29 16:45:33 by dtimoshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

#define ROTRIGHT64(a,b) (((a) >> (b)) | ((a) << (64-(b))))
#define EP0(x) (ROTRIGHT64(x,28) ^ ROTRIGHT64(x,34) ^ ROTRIGHT64(x,39))
#define EP1(x) (ROTRIGHT64(x,14) ^ ROTRIGHT64(x,18) ^ ROTRIGHT64(x,41))
#define SIG0(x) (ROTRIGHT64(x,1) ^ ROTRIGHT64(x,8) ^ ((x) >> 7))
#define SIG1(x) (ROTRIGHT64(x,19) ^ ROTRIGHT64(x,61) ^ ((x) >> 6))

static const unsigned long	g_512[] =
		{
				0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
				0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
				0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
				0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
				0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
				0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
				0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
				0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
				0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
				0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
				0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
				0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
				0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
				0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
				0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
				0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
				0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
				0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
				0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
				0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
				0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
				0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
				0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
				0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
				0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
				0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
				0x5fcb6fab3ad6faec, 0x6c44198c4a475817
		};

unsigned char	*msg_padding_384(t_content *string)
{
	size_t			bits;
	size_t			done_len;
	unsigned char	*done_str;
	int 			i;

	i = -1;
	bits = string->cont_len * 8;
	done_len = (((bits + 1024 / 8) / 1024) * 1024 + 1024) / 8;
	done_str = (unsigned char *)ft_memalloc(done_len);
	ft_memcpy(done_str, string->content, string->cont_len);
	done_str[string->cont_len] = 128;
	while (++i < 8)
		done_str[done_len - (8 - i)] = (unsigned char)(bits >> (8 * (7 - i)));
	string->cont_len = done_len;
	return (done_str);
}


static void					sha384_init_msg_schedule(
		unsigned long *m, const unsigned char *data)
{
	int i;
	int j;

	i = 0;
	while (i < 16)
	{
		j = 0;
		while (j < 8)
		{
			m[i] = (m[i] << 8) + *data;
			data++;
			j++;
		}
		i++;
	}
	while (i < 80)
	{
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
		i++;
	}
}

void						sha384_transform(
		unsigned long *state, const unsigned char *data)
{
	unsigned long	temp1;
	unsigned long	temp2;
	unsigned long	m[80];
	unsigned long	vars[8];
	int				i;

	sha384_init_msg_schedule(m, data);
	i = -1;
	while (++i < 8)
		vars[i] = state[i];
	i = -1;
	while (++i < 80)
	{
		temp1 = vars[7] + EP1(vars[4])
				+ CH(vars[4], vars[5], vars[6]) + g_512[i] + m[i];
		temp2 = EP0(vars[0]) + MAJ(vars[0], vars[1], vars[2]);
		sha512_swap(vars, temp1, temp2);
	}
	i = -1;
	while (++i < 8)
		state[i] += vars[i];
}


void	sha384_init(unsigned long *state)
{
	state[0] = 0xcbbb9d5dc1059ed8;
	state[1] = 0x629a292a367cd507;
	state[2] = 0x9159015a3070dd17;
	state[3] = 0x152fecd8f70e5939;
	state[4] = 0x67332667ffc00b31;
	state[5] = 0x8eb44a8768581511;
	state[6] = 0xdb0c2e0d64f98fa7;
	state[7] = 0x47b5481dbefa4fa4;
}

void	sha384(t_content *string)
{
//	char			*digest;
	unsigned long	state[8];
	unsigned char	*to_free;
	unsigned int	i;

	sha384_init(state);
	to_free = string->content;
	string->content = msg_padding_384(string);
	ft_memdel((void **)&to_free);
	i = 0;
	while (i < string->cont_len)
	{
		sha384_transform(state, string->content + i);
		i += 1024 / 8;
	}
	i = 0;
	while (i < 6)
		ft_printf("%016lx", state[i++]);
	ft_memdel((void **)&string->content);
}
