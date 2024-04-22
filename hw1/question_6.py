DISTANCE = 20_000 * 1000  # meters
LINK_BANDWITH = 2e6  # megabits per second
PROPAGATION_SPEED = 2.5 * 10**8  # meters per second
PACKET_SIZE = 800_000  # bits


print(f"It would take {(DISTANCE / PROPAGATION_SPEED) * 1000} msec to transmit")

print(
    f"to put all bits on the wire it would take {(PACKET_SIZE/LINK_BANDWITH) * 1000} msec"
)

print((80 / 400) * PACKET_SIZE)
