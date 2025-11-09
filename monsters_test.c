#include "libft.h"
#include "stdio.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	printf("----------------------------------------------------MONSETRS TESTS-----------------------------------------------------------------------------");

	printf("start testing ...");
	sleep(3);
	printf("\n-------------------ft_atoi ----------------------------------: \n");
	int	a;
	int	b;

	a = atoi("-2147483648");
	b = ft_atoi("-2147483648");
	sleep(1);
	printf("expected : %d ------------> yours : %d : ",a,b);
	sleep(1);
	a == b ? printf("✅\n") : printf("❌\n");
	a = atoi("2147483647");
	b = ft_atoi("2147483647");
	sleep(1);
	printf("expected : %d ------------> yours : %d : ",a,b);
	sleep(1);
	a == b ? printf("✅\n") : printf("❌\n");
	a = atoi("0");
	b = ft_atoi("0");
	sleep(1);
	printf("expected : %d ------------> yours : %d : ",a,b);
	sleep(1);
	a == b ? printf("✅\n") : printf("❌\n");
	a = INT_MAX;
	b = INT_MIN;
	ft_bzero(&a,4);
	ft_bzero(&b,4);
	sleep(2);
	printf("\n-------------------ft_bzero ----------------------------------: \n");
	sleep(1);
	printf("expected : %d ------------> yours : %d : ",0,a);
	sleep(1);
	a == 0 ? printf("✅\n") : printf("❌\n");
	sleep(1);
	printf("expected : %d ------------> yours : %d : ",0,a);
	sleep(1);
	b == 0 ? printf("✅\n") : printf("❌\n");
	printf("\n-------------------ft_calloc ----------------------------------: \n");
	int	*s = ft_calloc(5,4);
	int	*s2 = calloc(5,4);
	a = 0;
	while (a < 5)
	{
		sleep(1);
		printf("expected : %d ------------> yours : %d : ",s2[a],s[a]);
		s[a] == s2[a] ? printf("✅\n") : printf("❌\n");
		a++;
		sleep(1);
	}
	free(s);
	free(s2);
	s = calloc(0,0);
	s2 = calloc(0,0);
	 printf("expected : %s ------------> yours : %s : ",s2[a],s[a]);
	s[a] == s2[a] ? printf("✅\n") : printf("❌\n");
	free(s);
	free(s2);
	printf("\n-------------------isalnum ----------------------------------: \n");
	char *s1 = malloc(37);
	s1 = "1234567890qwertyuioplkjhgfdsazxcvbnm";
	a = 0;
	while (a < 36)
	{
		b = ft_isalnum(s1[a]);
		sleep(1);
		printf("expected : %d ------------> yours : %d : ",1,b);
		b == 1 ?  printf("✅\n") : printf("❌\n");
		a++;
		sleep(1);
	}

}
