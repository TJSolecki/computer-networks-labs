PROPAGATION_SPEED = 2.5 * (10**8)
SWITCH_DELAY = 3
PACKET_SIZE = 1_500
TRANSMITION_RATE = 312_500


def get_link_delay(link_length_in_km: int):
    # units seconds
    transmition_delay = PACKET_SIZE / TRANSMITION_RATE

    # units seconds
    propagation_delay = (link_length_in_km * 1000) / PROPAGATION_SPEED

    # units milliseconds
    return transmition_delay * 1000 + propagation_delay * 1000


print(
    get_link_delay(5000)
    + get_link_delay(4000)
    + get_link_delay(1000)
    + 2 * SWITCH_DELAY
)
