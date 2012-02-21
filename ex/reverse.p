struct Node {
    struct Node* next
    int data
}

void reverse(Node* const current, Node* const out) {

    Node* tmp;
    out = NULL;

    while(current != NULL) {
        tmp = current->next;
        current->next = out;
        out = current;
        current = tmp;
    }
}
