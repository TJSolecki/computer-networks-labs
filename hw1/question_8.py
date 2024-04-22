from math import comb

p = 0.1  # percent chance a user is using the link


def calc_prob_n_of_m_users_are_using_link(n: int, m: int):
    return comb(20, 9) * (p**n) * ((1 - p) ** (m - n))


print(calc_prob_n_of_m_users_are_using_link(9, 20))
