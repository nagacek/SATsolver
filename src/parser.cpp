//
// Created by natalia on 31.05.23.
//
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "parser.h"
#include "exceptions/sat_exception.h"

namespace parser {
    std::istream &next_word(std::istream &stream, std::string &to_store) {
        stream >> std::ws;
        return getline(stream, to_store, ' ');
    }

    int wtoi(const std::string &word) {
        size_t point;
        int var_num = std::stoi(word, &point);
        if (point != word.size())
            throw invalid_format_exception(std::string("not a number"));
        return var_num;
    }

    sat_bool parse_clauses(ifstream &stream, cnf *cnf_val) {
        std::string line;
        std::string word;
        bool new_clause = false;
        vector<lit> clause_lits;
        //weak_ptr<clause> clause_ptr;
        while (std::getline(stream, line)) {
            std::istringstream line_stream(line);
            while (next_word(line_stream, word)) {
                if (new_clause) {
                    new_clause = false;
                }
                int value = wtoi(word);
                if (value == 0) {
                    new_clause = true;
                    cnf_val->add_clause(clause_lits);
                    clause_lits.clear();
                    continue;
                }
                lit new_lit = lit(value);
                clause_lits.push_back(new_lit);
            }
        }
        return sat_bool::Undef;
    }

    std::string skip_comments(std::ifstream &source) {
        std::string comment_line;
        do {
            std::getline(source, comment_line);
        } while (comment_line.at(0) == 'c');
        return comment_line;
    }

    void parse_first(const std::string &first_line, assignment *assgn, cnf *cnf_val, watch_list *wl) {
        std::istringstream line(first_line);
        std::string word;

        next_word(line, word);
        if (word != "p")
            throw invalid_format_exception(std::string("preamble not correctly indicated"));

        next_word(line, word);
        if (word != "cnf")
            throw invalid_format_exception(std::string("not a cnf"));

        next_word(line, word);
        if (word.empty())
            throw invalid_format_exception("invalid variable count declaration");
        unsigned var_num = wtoi(word);
        assgn->set_var_num((int)var_num);
        wl->init(var_num);

        next_word(line, word);
        if (word.empty())
            throw invalid_format_exception("invalid clause count declaration");
        cnf_val->set_clause_num(wtoi(word));
    }

    std::string parse(int argc, char **argv, solver &s) {
        if (argc < 1) {
            throw invalid_arg_exception(std::string("argument list is too small"));
        }
        std::ifstream source;
        std::string cnf_file = std::string(argv[1]);
        source.open(cnf_file);
        if (!source.is_open()) {
            throw invalid_file_exception();
        }
        logger::log(logger::INFO, "------ Instance \"" + cnf_file + "\" ------");
        logger::log(logger::INFO, "Parsing");
        std::string line;

        assignment *assgn = s.get_assignment();
        cnf *cnf_val = s.get_formula();

        parse_first(skip_comments(source), assgn, cnf_val, s.get_watch_list());

        parse_clauses(source, cnf_val);
        return cnf_file;
    }

    void unparse(assignment *assgn, const string& cnf_file) {
        std::string cnf_sol_file = cnf_file + ".sol";
        std::string dimacs_sol = "p cnf " + std::to_string(assgn->get_var_num()) + " " + std::to_string(assgn->get_var_num());

        for (int i = 1; i <= assgn->get_var_num(); i++) {
            dimacs_sol += "\n";
            sat_bool curr_assgn = assgn->get_assignment(i);
            if (curr_assgn == sat_bool::True) {
                dimacs_sol += to_string(i);
            } else if (curr_assgn == sat_bool::False) {
                dimacs_sol += "-" + to_string(i);
            } else {
                log(logger::type::ERROR, "Variable [" + to_string(i) + "] was not assigned when finished.");
                exit(-1);
            }
        }

        std::ofstream outfile;
        outfile.open(cnf_sol_file);
        outfile << dimacs_sol;
    }
}
