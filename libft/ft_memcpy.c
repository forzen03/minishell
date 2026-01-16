/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:41:54 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:41:54 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dst;
	unsigned char	*sr;

	i = 0;
	dst = (unsigned char *)dest;
	sr = (unsigned char *)src;
	if (src == NULL && dest == NULL)
		return (NULL);
	while (i < n)
	{
		dst[i] = sr[i];
		i++;
	}
	return (dest);
}

/*void print_bytes(char *label, unsigned char *buf, size_t n)
{
	printf("%s: ", label);
	for (size_t i = 0; i < n; i++)
		printf("%02x ", buf[i]);
	printf("\n");
}

int main(void)
{
	char dest[20];
	char src[20];
	char *large_src;
	char *large_dest;

	// 1. Copy zero bytes
	strcpy(dest, "world");
	strcpy(src, "hello");
	ft_memcpy(dest, src, 0);
	printf("1. Copy zero bytes:\n%s\n\n", dest); // expect "world"

	// 2. Copy with same src and dest
	strcpy(dest, "hello");
	ft_memcpy(dest, dest, 5);
	printf("2. Copy same src and dest:\n%s\n\n", dest); // expect "hello"

	// 3. Partial copy
	strcpy(dest, "aaaaa");
	strcpy(src, "hello");
	ft_memcpy(dest, src, 3);
	dest[5] = '\0'; // null terminate to print as string
	printf("3. Partial copy (3 bytes):\n%s\n\n", dest); // expect "helaa"

	// 4. Copy including null terminator
	strcpy(dest, "aaaaaa");
	strcpy(src, "hello");
	ft_memcpy(dest, src, 6);
	printf("4. Copy including null terminator:\n%s\n\n", dest); // expect "hello"

	// 5. Overlapping buffers (undefined behavior) - Demonstration only!
	strcpy(dest, "1234567890");
	ft_memcpy(dest + 2, dest, 5);
	printf("5. Overlapping buffers (undefined behavior):\n%s\n\n", dest);
	// Result unpredictable, may corrupt data

	// 6. NULL pointers (unsafe, just demo, expect crash or segfault)
	// Uncomment to test, but will crash your program:
	// ft_memcpy(NULL, "hello", 5);
	// ft_memcpy("hello", NULL, 5);

	// 7. Large buffer copy
	large_src = malloc(1000000);
	large_dest = malloc(1000000);
	if (!large_src || !large_dest)
	{
		printf("Failed to allocate large buffers.\n");
		return (1);
	}
	memset(large_src, 'A', 1000000);
	ft_memcpy(large_dest, large_src, 1000000);
	printf("7. Large buffer copy: first 10 bytes: ");
	for (int i = 0; i < 10; i++)
		printf("%c", large_dest[i]); // expect "AAAAAAAAAA"
	printf("\n");

	free(large_src);
	free(large_dest);

	return (0);
}*/