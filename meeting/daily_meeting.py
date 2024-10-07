from datetime import datetime

# Function to get the current date or prompt for it
def get_current_date():
	date_input = input("Enter the date (YYYY-MM-DD) or press Enter to use today's date: ")
	if date_input.strip() == "":
		return datetime.now().strftime("%Y-%m-%d")
	else:
		return date_input

# Function to create a log entry for a single team member
def create_log_entry(name):
	print(f"\nCreating log for {name}")
	done = input(f"What have you done? ")
	will_do = input(f"What will you do? ")
	remarks = input("Any special remarks or notes? ")

	# Format the log entry
	log_entry = f"Name: {name}\nWhat have you done?\n- {done}\nWhat will you do?\n- {will_do}\nSpecial Remarks: {remarks}\n{'-'*40}\n"
	return log_entry

# Function to save the log entries to a file
def save_log_entries(entries):
	with open("meeting_log.txt", "a") as file:
		file.write(entries)

def main():
	# Get the date
	date = get_current_date()
	# Initialize the log entries string
	log_entries = f"Date: {date}\n{'='*40}\n"
	# Loop to prompt for 5 team members
	for i in range(5):
		name = input(f"\nEnter name for team member {i+1}: ")
		log_entry = create_log_entry(name)
		log_entries += log_entry

	# Save the entries to the file
	save_log_entries(log_entries)
	print("\nLog entries saved successfully.")

if __name__ == "__main__":
	main()
