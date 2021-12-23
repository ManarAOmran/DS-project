#include "XML_TO_JSON.h"

int XML_TO_JSON(Node *root, int level, string *input, vector<Node *> children_data_arr)
{

	if (root->get_name() == COMMENT_TAG || root->get_name() == "XML,TAG")
	{
		return 0;
	}

	// Copying Root children
	vector<Node *> children = root->get_children();

	map<string, vector<Node *>> children_data_map;
	map<string, int> children_data_wait;
	int remaining_children = 0;

	if (children.size())
	{
		for (auto child : children)
		{
			// if the the child wasnt a comment or xml tag
			// we put them in the map to trace them
			if (child->get_name() != COMMENT_TAG && root->get_name() != "XML,TAG")
			{
				children_data_map[child->get_name()].push_back(child);
				children_data_wait[child->get_name()] = 1;
			}
		}
	}


	for (int i = 0; i < level + 1; i++)
	{
		*input += "    ";
	}

	// if its the beginng of the xml
	if (root->get_name() == "XML")
	{
		/*
		 * print the first "{" in the json
		 */
		*input += "{\n";
	}
	else
	{
		// if t wasn`t the first line and it has children we print its name and [{\n
		if (children.size() || root->get_attr().size())
		{
			if (children_data_arr.size() > 1)
			{
				if (root == children_data_arr.front())
				{
					//printing<< "root name": [{
					*input += "\"" + root->get_name() + "\": [{\n";
				}
				else
				{
					*input += "{\n";
				}
			}
			else // if it has no children or attributes
			{
				*input += "\"" + root->get_name() + "\": {\n";
			}

			/*************************************
			 *
			 * printing attributes
			 *
			 *************************************/
			if (root->get_attr().size())
			{
				for (auto attr : root->get_attr())
				{
					//printing white spaces to represent levels
					for (int i = 0; i < level + 2; i++)
					{
						*input += "    ";
					}


					/*
					 * adding attribute key highlighted and its value
					   printing "@attribute key" : "value"
					 */

					*input += "\"@" + attr.key + "\": " + "\"" + attr.value + "\"";

					//if there are no attributes print new line

					if (attr.key == root->get_attr().back().key && attr.value == root->get_attr().back().value && root->get_data().empty() && children.empty())
					{
						*input += "\n";
					}
					//else printing "," and newline to continue printing attributes
					else
					{
						*input += ",\n";
					}
				}

				if (!root->get_data().empty())
				{
					size_t index = 0;
					while (true)
					{

						index = root->get_data().find("\"", index);

						//if it doesn`t have "
						if (index == string::npos)
							break;

						string data = root->get_data();
						//remove " and adding \" instead
						data.replace(index, index + 1, "\\\"");
						root->set_data(data);
						index += 2;
					}
					for (int i = 0; i < level + 2; i++)
					{
						*input += "    ";
					}
					*input += "\"title\": \"" + root->get_data() + "\"\n";
					for (int i = 0; i < level + 1; i++)
					{
						*input += "    ";
					}
					*input += "}";
				}
			}
		}


		else
		{
			size_t index = 0;
			while (true)
			{
				index = root->get_data().find("\"", index);
				if (index == std::string::npos)
					break;
				std::string data = root->get_data();
				data.replace(index, index + 1, "\\\"");
				root->set_data(data);
				index += 2;
			}
			if (children_data_arr.size() > 1)
			{
				if (root == children_data_arr.front())
				{
					*input += "\"" + root->get_name() + "\": [" + "\"" + root->get_data() + "\"";
				}
				else
				{
					*input += "\"" + root->get_data() + "\"";
				}
			}
			else
			{
				*input += "\"" + root->get_name() + "\": " + "\"" + root->get_data() + "\"";
			}
		}
	}

	/*
	 *
	 */

	for (int i = 0; i < children.size(); i++)
	{
		if (children[i]->get_name() != COMMENT_TAG)
		{
			if (children_data_wait[children[i]->get_name()] == 0)
			{
				continue;
			}

			/*
			 * calculating the total number of children this root have
			 */

			int remaning_children = std::accumulate(children_data_wait.begin(), children_data_wait.end(), 0, [](int value, const std::map<std::string, int>::value_type &p)
					{ return value + p.second; });
			for (auto child : children_data_map[children[i]->get_name()])
			{
				int status_check = XML_TO_JSON(child, level + 1, input, children_data_map[children[i]->get_name()]);
				if (status_check)
				{
					if ((remaning_children == 1 && child == children_data_map[children[i]->get_name()].back()) || children.size() == 1)
					{
						*input += "\n";
					}
					else
					{
						*input += ",\n";
					}
				}
			}
			children_data_wait[children[i]->get_name()] = 0;
		}
	}

	if (root->get_name() == "XML")
	{
		*input += "}";
	}
	else
	{
		if (root->get_data().empty())
		{

			for (int i = 0; i < level + 1; i++)
			{
				*input += "    ";
			}
			if (children_data_arr.size() > 1 && children_data_arr.back() == root)
			{
				*input += "}]";
			}
			else
			{
				*input += "}";
			}
		}
		else
		{
			if (children_data_arr.size() > 1 && children_data_arr.back() == root)
			{
				*input += "]";
			}
		}
	}
	return 1;
}

