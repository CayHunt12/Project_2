import csv

def process_feed_data(file_path):
  try:
    values = []
    with open(file_path, mode='r') as file:
      header = file.readline()
      header_columns = header.strip().split(",")

      if "value" not in header_columns:
        print("Error: The file does not contain a 'value' column.")
        return

      value_index = header_columns.index("value")

      for line in file:
        columns = line.strip().split(",")

        if len(columns) > value_index:
          try:
            value = float(columns[value_index].strip())
            values.append(value)
          except ValueError:
            print("Skipping invalid row: {}".format(line.strip()))
       


      if len(values) > 0:
         min_value = values[0]
         for value in values:
           if value < min_value:
             min_value = value
        
         max_value = values[0]
         for value in values:
           if value > max_value:
             max_value = value

         total = 0
         for value in values:
           total += value
         avg_value = total / len(values)
         print("Temperature Report")
         print("------------------")
         print("Minimum Temperature: {:.2f} C".format(min_value))
         print("Maximum Temperature: {:.2f} C".format(max_value))
         print("Average Temperature: {:.2f} C".format(avg_value))
      
      else:
         print("No valid temperature data found")

  except FileNotFoundError:
    print("Error: File '{}' not found.".format(file_path))
  except Exception as e:
    print("An unexpected error occurred: {}".format(e))

if __name__ == "__main__":
  file_path = "feed_data.csv"
  process_feed_data(file_path)



